Name:		org.tizen.ug-lockscreen-options
Summary:	App: org.tizen.ug-lockscreen-options
Version:	0.1.108
Release:	1
Group:		Applications/Core Applications
License:	Apache-2.0
Source0:	%{name}-%{version}.tar.gz

%define cflags	" -Wall -Wextra -g -fPIC -O3 -Wno-unused-parameter -Wno-missing-field-initializers -fvisibility=hidden -finstrument-functions -Wl,--as-needed -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64"
%define cxxflags	" -Wall -Wextra -g -fPIC -O3 -Wno-unused-parameter -Wno-missing-field-initializers -fvisibility=hidden -fvisibility-inlines-hidden -finstrument-functions -Wl,--as-needed -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64"

%if "%{?tizen_profile_name}"=="wearable"
ExcludeArch: %{arm} %ix86 x86_64
%endif

%if "%{?tizen_profile_name}"=="tv"
ExcludeArch: %{arm} %ix86 x86_64
%endif

%define PREFIX           %{TZ_SYS_RO_APP}/%{name}

BuildRequires: cmake
BuildRequires: gettext-tools
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(appcore-efl)
BuildRequires: pkgconfig(evas)
BuildRequires: pkgconfig(ecore)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(capi-appfw-app-control)
BuildRequires: pkgconfig(capi-system-system-settings)
BuildRequires: pkgconfig(eina)
BuildRequires: pkgconfig(efl-extension)
BuildRequires: pkgconfig(libtzplatform-config)

%description
org.tizen.ug-lockscreen-options

%prep
%setup -q -n %{name}-%{version}

%build

%if 0%{?sec_build_binary_debug_enable}
export CFLAGS="$CFLAGS -DTIZEN_DEBUG_ENABLE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
export FFLAGS="$FFLAGS -DTIZEN_DEBUG_ENABLE"
%endif

CFLAGS+=%cflags
CXXFLAGS+=%cxxflags

cmake . -DCMAKE_INSTALL_PREFIX=%{PREFIX} \
		-DTZ_SYS_RO_PACKAGES=%{TZ_SYS_RO_PACKAGES}


make -j1

%install
rm -rf %{buildroot}
%make_install

%post
GOPTION="-g 6514"

%files
%manifest org.tizen.ug-lockscreen-options.manifest

%{PREFIX}/*
%{TZ_SYS_RO_PACKAGES}/*
