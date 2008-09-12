%define doomname prboom

Name: %{doomname}
Summary: Doom - classic 3D shoot-em-up game
Version: 2.4.7
Release: 1
Source: ftp://prdownloads.sourceforge.net/prboom/prboom-2.4.7.tar.gz
URL: http://prboom.sourceforge.net/
Group: Amusements/Games
BuildRoot: /var/tmp/%{doomname}-buildroot
License: GPL
Packager: Colin Phipps <cph@moria.org.uk>
Prefix: %{_prefix}
Autoreq: 0
Requires: libSDL-1.2.so.0, libSDL_mixer-1.2.so.0, libSDL_net-1.2.so.0, libm.so.6, libc.so.6

%description
Doom is the classic 3D shoot-em-up game. It must have been one of the best
selling games ever; it totally outclassed any  3D world games that preceded
it, with amazing speed, flexibility, and outstanding gameplay. The specs to
the game were released, and thousands of extra levels were written by fans of
the game; even today new levels are written for Doom faster then any one person
could play them. 

%prep
rm -rf $RPM_BUILD_ROOT

%setup -q

%build
%configure --disable-cpu-opt
make

%install
make %{?buildroot:DESTDIR=%{buildroot}} install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%{_mandir}/man5
%{_mandir}/man6
%docdir %{_docdir}
%{_docdir}
/usr/games
%{_datadir}/games/doom/prboom.wad

