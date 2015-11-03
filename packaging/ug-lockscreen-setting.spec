Name:		ug-lockscreen-options
Summary:	UI Gadget: ug-lockscreen-options
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

BuildRequires: cmake
BuildRequires: gettext-tools
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(appcore-efl)
BuildRequires: pkgconfig(ui-gadget-1)
BuildRequires: pkgconfig(evas)
BuildRequires: pkgconfig(ecore)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(capi-system-system-settings)
BuildRequires: pkgconfig(eina)
BuildRequires: pkgconfig(efl-extension)

%description
ug-lockscreen-options

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

cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} \


make -j1

%install
rm -rf %{buildroot}
%make_install

%post
GOPTION="-g 6514"

mkdir -p /usr/ug/bin/
ln -sf /usr/bin/ug-client /usr/ug/bin/setting-lockscreen-options-efl

%files
%manifest ug-lockscreen-options.manifest

/usr/ug/lib/*
/usr/ug/res/*
/usr/share/packages/*