%define doomname opendoom

Name: %{doomname}
Summary: Doom - classic 3D shoot-em-up game
Version: 0.1
Release: 1
Source: http://code.google.com/p/opendoom/
URL: http://unsyncopated.com/BrainSolvent/Doom%20Port
Group: Mobile/Games
BuildRoot: /var/tmp/%{doomname}-buildroot
License: GPL
Packager: Scott Carlson <scottrcarlson@gmail.com>
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
