OS_VER=$( grep VERSION_ID /etc/os-release | cut -d'=' -f2 | sed 's/[^0-9\.]//gI' \
| cut -d'.' -f1 )

MEM_MEG=$( free -m | sed -n 2p | tr -s ' ' | cut -d\  -f2 )
CPU_SPEED=$( lscpu | grep "MHz" | tr -s ' ' | cut -d\  -f3 | cut -d'.' -f1 )
CPU_CORE=$( nproc )
MEM_GIG=$(( ((MEM_MEG / 1000) / 2) ))
export JOBS=$(( MEM_GIG > CPU_CORE ? CPU_CORE : MEM_GIG ))

DISK_INSTALL=$( df -h . | tail -1 | tr -s ' ' | cut -d\  -f1 )
DISK_TOTAL_KB=$( df . | tail -1 | awk '{print $2}' )
DISK_AVAIL_KB=$( df . | tail -1 | awk '{print $4}' )
DISK_TOTAL=$(( DISK_TOTAL_KB / 1048576 ))
DISK_AVAIL=$(( DISK_AVAIL_KB / 1048576 ))

printf "\\nOS name: ${OS_NAME}\\n"
printf "OS Version: ${OS_VER}\\n"
printf "CPU speed: ${CPU_SPEED}Mhz\\n"
printf "CPU cores: ${CPU_CORE}\\n"
printf "Physical Memory: ${MEM_MEG}Mgb\\n"
printf "Disk install: ${DISK_INSTALL}\\n"
printf "Disk space total: ${DISK_TOTAL%.*}G\\n" 
printf "Disk space available: ${DISK_AVAIL%.*}G\\n"
printf "Concurrent Jobs (make -j): ${JOBS}\\n"

if [ "${MEM_MEG}" -lt 7000 ]; then
	printf "\\nYour system must have 7 or more Gigabytes of physical memory installed.\\n"
	printf "Exiting now.\\n\\n"
	exit 1;
fi

if [ "${OS_VER}" -lt 7 ]; then
	printf "\\nYou must be running Centos 7 or higher to install EOSIO.\\n"
	printf "Exiting now.\\n\\n"
	exit 1;
fi

if [ "${DISK_AVAIL%.*}" -lt "${DISK_MIN}" ]; then
	printf "\\nYou must have at least %sGB of available storage to install EOSIO.\\n" "${DISK_MIN}"
	printf "Exiting now.\\n\\n"
	exit 1;
fi

printf "\\n"

printf "Checking Yum installation...\\n"
if ! YUM=$( command -v yum 2>/dev/null ); then
		printf "!! Yum must be installed to compile EOS.IO !!\\n"
		printf "Exiting now.\\n"
		exit 1;
fi
printf " - Yum installation found at %s.\\n" "${YUM}"

printf "\\nDo you wish to update YUM repositories?\\n\\n"
select yn in "Yes" "No"; do
	case $yn in
		[Yy]* ) 
			printf "\\n\\nUpdating...\\n\\n"
			if ! sudo $YUM -y update; then
				printf "\\nYUM update failed.\\n"
				printf "\\nExiting now.\\n\\n"
				exit 1;
			else
				printf "\\nYUM update complete.\\n"
			fi
		break;;
		[Nn]* ) 
			echo "Proceeding without update!"
		break;;
		* ) echo "Please type 1 for yes or 2 for no.";;
	esac
done

printf "Checking installation of Centos Software Collections Repository...\\n"
SCL=$( rpm -qa | grep -E 'centos-release-scl-[0-9].*' )
if [ -z "${SCL}" ]; then
	printf " - Do you wish to install and enable this repository?\\n"
	select yn in "Yes" "No"; do
		case $yn in
			[Yy]* )
				printf "Installing SCL...\\n"
				if ! sudo $YUM -y --enablerepo=extras install centos-release-scl; then
					printf "!! Centos Software Collections Repository installation failed !!\\n"
					printf "Exiting now.\\n\\n"
					exit 1;
				else
					printf "Centos Software Collections Repository installed successfully.\\n"
				fi
			break;;
			[Nn]* ) echo "User aborting installation of required Centos Software Collections Repository, Exiting now."; exit;;
			* ) echo "Please type 1 for yes or 2 for no.";;
		esac
	done
else
	printf " - ${SCL} found.\\n"
fi

printf "Checking installation of devtoolset-8...\\n"
DEVTOOLSET=$( rpm -qa | grep -E 'devtoolset-8-[0-9].*' )
if [ -z "${DEVTOOLSET}" ]; then
	printf "Do you wish to install devtoolset-8?\\n"
	select yn in "Yes" "No"; do
		case $yn in
			[Yy]* )
				printf "Installing devtoolset-8...\\n"
				if ! sudo $YUM install -y devtoolset-8 2>/dev/null; then
						printf "!! Centos devtoolset-8 installation failed !!\\n"
						printf "Exiting now.\\n"
						exit 1;
				else
						printf "Centos devtoolset installed successfully.\\n"
				fi
			break;;
			[Nn]* ) echo "User aborting installation of devtoolset-8. Exiting now."; exit;;
			* ) echo "Please type 1 for yes or 2 for no.";;
		esac
	done
else
	printf " - ${DEVTOOLSET} found.\\n"
fi
printf "Enabling Centos devtoolset-8...\\n"
if ! source "/opt/rh/devtoolset-8/enable" 2>/dev/null; then
	printf "!! Unable to enable Centos devtoolset-8 at this time !!\\n"
	printf "Exiting now.\\n\\n"
	exit 1;
fi
printf "Centos devtoolset-8 successfully enabled.\\n"


printf "Checking installation of llvm-toolset-7.0...\\n"
DEVLLVMTOOLSET=$( rpm -qa | grep -E 'llvm-toolset-7.0-[0-9].*' )
if [ -z "${DEVLLVMTOOLSET}" ]; then
	printf "Do you wish to install llvm-toolset-7.0?\\n"
	select yn in "Yes" "No"; do
		case $yn in
			[Yy]* )
				printf "Installing llvm-toolset-7.0...\\n"
				if ! sudo $YUM install -y llvm-toolset-7.0 2>/dev/null; then
						printf "!! Centos llvm-toolset-7.0 installation failed !!\\n"
						printf "Exiting now.\\n"
						exit 1;
				else
						printf "Centos llvm-toolset installed successfully.\\n"
				fi
			break;;
			[Nn]* ) echo "User aborting installation of llvm-toolset-7.0. Exiting now."; exit;;
			* ) echo "Please type 1 for yes or 2 for no.";;
		esac
	done
else
	printf " - ${DEVLLVMTOOLSET} found.\\n"
fi
printf "Enabling Centos llvm-toolset-7.0...\\n"
if ! source "/opt/rh/llvm-toolset-7.0/enable" 2>/dev/null; then
	printf "!! Unable to enable Centos llvm-toolset-7.0 at this time !!\\n"
	printf "Exiting now.\\n\\n"
	exit 1;
fi
printf "Centos llvm-toolset-7.0 successfully enabled.\\n"

printf "\\n"

DEP_ARRAY=( 
	git autoconf automake libtool make bzip2 \
	bzip2-devel openssl-devel gmp-devel \
	ocaml libicu-devel python python-devel python33 \
	gettext-devel file sudo rpm-build llvm-toolset-7.0-cmake
)
COUNT=1
DISPLAY=""
DEP=""
printf "Checking RPM for installed dependencies...\\n"
for (( i=0; i<${#DEP_ARRAY[@]}; i++ )); do
	pkg=$( rpm -qi "${DEP_ARRAY[$i]}" 2>/dev/null | grep Name )
	if [[ -z $pkg ]]; then
		DEP=$DEP" ${DEP_ARRAY[$i]} "
		DISPLAY="${DISPLAY}${COUNT}. ${DEP_ARRAY[$i]}\\n"
		printf "!! Package %s ${bldred} NOT ${txtrst} found !!\\n" "${DEP_ARRAY[$i]}"
		(( COUNT++ ))
	else
		printf " - Package %s found.\\n" "${DEP_ARRAY[$i]}"
		continue
	fi
done
printf "\\n"
if [ "${COUNT}" -gt 1 ]; then
	printf "The following dependencies are required to install EOSIO.\\n"
	printf "${DISPLAY}\\n\\n"
	printf "Do you wish to install these dependencies?\\n"
	select yn in "Yes" "No"; do
		case $yn in
			[Yy]* )
				printf "Installing dependencies\\n\\n"
				if ! sudo $YUM -y install ${DEP}; then
					printf "!! YUM dependency installation failed !!\\n"
					printf "Exiting now.\\n"
					exit 1;
				else
					printf "YUM dependencies installed successfully.\\n"
				fi
			break;;
			[Nn]* ) echo "User aborting installation of required dependencies, Exiting now."; exit;;
			* ) echo "Please type 1 for yes or 2 for no.";;
		esac
	done
else
	printf " - No required YUM dependencies to install.\\n"
fi


printf "\\n"


printf "Checking CMAKE installation...\\n"
# Find and use existing CMAKE
export CMAKE=$(command -v cmake 2>/dev/null)
if [ -z $CMAKE ]; then
	printf "Installing CMAKE...\\n"
	curl -LO https://cmake.org/files/v$CMAKE_VERSION_MAJOR.$CMAKE_VERSION_MINOR/cmake-$CMAKE_VERSION.tar.gz \
	&& tar -xzf cmake-$CMAKE_VERSION.tar.gz \
	&& cd cmake-$CMAKE_VERSION \
	&& ./bootstrap --prefix=${ROOT_LOCATION} \
	&& make -j"${JOBS}" \
	&& make install \
	&& cd .. \
	&& rm -f cmake-$CMAKE_VERSION.tar.gz \
	|| exit 1
	export CMAKE=$ROOT_LOCATION/bin/cmake
	printf " - CMAKE successfully installed @ ${CMAKE}.\\n"
else
	printf " - CMAKE found @ ${CMAKE}.\\n"
fi