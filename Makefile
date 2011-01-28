all: qt kde

qt:
	mkdir -p build/qt; mkdir -p install/qt
	cd build/qt; cmake ../.. -DCMAKE_BUILD_TYPE=DebugFull -DCMAKE_INSTALL_PREFIX=../../install/qt -DQTONLY=1 && make && make install
	
kde:
	mkdir -p build/kde; mkdir -p install/kde
	cd build/kde; cmake ../.. -DCMAKE_BUILD_TYPE=DebugFull -DCMAKE_INSTALL_PREFIX=../../install/kde -DQTONLY=0 && make && make install

rq: qt
	LD_LIBRARY_PATH="./install/qt/lib" LC_ALL="de_DE.utf8" ./install/qt/bin/knipptasch &

rk: kde
	LD_LIBRARY_PATH="./install/kde/lib" LC_ALL="de_DE.utf8" ./install/kde/bin/knipptasch &

clean:
	find . -iname *~ -exec rm {} \;
	rm -rf ~/.config/Hilefoks
	rm -f ~/.kde/share/config/knipptaschrc
	rm -rf ~/.kde/share/apps/knipptasch
	rm -rf install
	rm -rf build
