MRuby::Build.new do |conf|
  toolchain :gcc

  conf.gembox 'default'
end

SIM_SYSROOT="#{SIMSDKPATH}"
DEVICE_SYSROOT="#{IOSSDKPATH}"

MRuby::CrossBuild.new('ios-simulator') do |conf|
  conf.bins = []

  conf.gembox 'default'

  conf.cc do |cc|
    cc.command = 'xcrun'
    cc.flags = %W(-sdk iphoneos llvm-gcc-4.2 -arch i386 -isysroot \#{SIM_SYSROOT} -g -O3 -Wall -Werror-implicit-function-declaration)
  end

  conf.linker do |linker|
    linker.command = 'xcrun'
    linker.flags = %W(-sdk iphoneos llvm-gcc-4.2 -arch i386 -isysroot \#{SIM_SYSROOT})
  end
end

MRuby::CrossBuild.new('ios-armv7') do |conf|
  conf.bins = []

  conf.gembox 'default'

  conf.cc do |cc|
    cc.command = 'xcrun'
    cc.flags = %W(-sdk iphoneos llvm-gcc-4.2 -arch armv7 -isysroot \#{DEVICE_SYSROOT} -g -O3 -Wall -Werror-implicit-function-declaration)
  end

  conf.linker do |linker|
    linker.command = 'xcrun'
    linker.flags = %W(-sdk iphoneos llvm-gcc-4.2 -arch armv7 -isysroot \#{DEVICE_SYSROOT})
  end
end

MRuby::CrossBuild.new('ios-armv7s') do |conf|
  conf.bins = []

  conf.gembox 'default'
  conf.cc do |cc|
    cc.command = 'xcrun'
    cc.flags = %W(-sdk iphoneos llvm-gcc-4.2 -arch armv7s -isysroot \#{DEVICE_SYSROOT} -g -O3 -Wall -Werror-implicit-function-declaration)
  end

  conf.linker do |linker|
    linker.command = 'xcrun'
    linker.flags = %W(-sdk iphoneos llvm-gcc-4.2 -arch armv7s -isysroot \#{DEVICE_SYSROOT})
  end
end
