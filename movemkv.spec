Name:           movemkv
Version:        0.1.1
Release:        1%{?dist}
Summary:        A small utility for moving certain MKV files.

License:        GPLv3
URL:            https://github.com/iAmSomeone2/movemkv
Source0:        https://github.com/iAmSomeone2/%{name}-alpha/archive/v%{version}.tar.gz

BuildRequires:  clang, cmake, make    

%description
A small utility for quickly and easily moving MKV files that use a specific naming scheme.

%prep
%setup -q


%build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
%make_install


%files
%license LICENSE
%{_bindir}/%{name}/%{name}



%changelog
* Sun Dec 15 2019 Brenden Davidson <davidson.brenden15@gmail.com>
- First movemkv package
