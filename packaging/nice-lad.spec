Name:       nice-lad
Summary:    Narcissistic, Incredible, Completely Exceptional Logger of Access Denials
Version:    0.1.0
Release:    1
Group:      Security/Access Control
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1001:    %{name}.manifest
Source1002:    nice-lad-tests.manifest
BuildRequires: boost-devel
BuildRequires: cmake
BuildRequires: pkgconfig(audit)
BuildRequires: pkgconfig(libsystemd-journal)
BuildRequires: pkgconfig(security-manager)

%{?systemd_requires}

%if !%{defined build_type}
%define build_type RELEASE
%endif

%description
Narcissistic, Incredible, Completely Exceptional Logger of Access Denials.
The tool collects logs of access denials on platform (syscalls, Smack, Cynara
and Nether). It also reasonably populates audit's filters, so there is
no unneeded impact on performance. The tool works as a plugin for audit and is
only spawned on demand. nice-lad could be helpful for developers trying
to debug their applications.

%package tests
Summary:    Unit tests for nice-lad
BuildRequires: pkgconfig(gmock)

%description tests
Unit tests for nice-lad. A standalone application checking correctness
of nice-lad's components.

%prep
%setup -q
cp -a %{SOURCE1001} .
cp -a %{SOURCE1002} .

%build
%if 0%{?sec_build_binary_debug_enable}
export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
%endif

%if %{?build_type} == "DEBUG"
export CXXFLAGS="$CXXFLAGS -Wp,-U_FORTIFY_SOURCE"
%endif

export LDFLAGS+="-Wl,--rpath=%{_libdir}"

%cmake . \
        -DCMAKE_BUILD_TYPE=%{?build_type} \
        -DBIN_DIR:PATH=%{_bindir} \
        -DSBIN_DIR:PATH=%{_sbindir} \
        -DSYS_CONFIG_DIR:PATH=%{_sysconfdir} \
        -DCMAKE_VERBOSE_MAKEFILE=ON \
        -DWITH_SECURITY_MANAGER=ON \
        -DWITH_JOURNALD=ON
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%files
%manifest %{name}.manifest
%license LICENSE
%attr(755,root,root) %{_sbindir}/%{name}
%config %{_sysconfdir}/audisp/plugins.d/nice_lad.conf

%files -n nice-lad-tests
%manifest nice-lad-tests.manifest
%license LICENSE
%attr(755,root,root) %{_bindir}/nice-lad-tests

