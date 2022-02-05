pkgname=eos-quickstart
pkgver=0.1
pkgrel=1
pkgdesc="An application for getting a quick start with installing packages"
arch=('x86_64')
license=('GPL3')
depends=('qt6-base' 'polkit')
optdepends=('konsole' 'gnome-terminal' 'xfce4-terminal' 'xterm' 'alacritty' 'lxterminal')
makedepends=('git' 'cmake' 'qt6-tools')
backup=(etc/eos-quickstart.conf)
source=(git+https://github.com/endeavouros-team/$pkgname.git)
md5sums=('SKIP')

build() {
    cd "$srcdir/$pkgname"
    cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE='Release'
    make -C build
}

package() {
    cd "$srcdir/$pkgname"
    make -C build DESTDIR="$pkgdir" install

    install -Dm0644 "$srcdir/$pkgname/eos-quickstart.conf" "$pkgdir/etc/eos-quickstart.conf"
}
