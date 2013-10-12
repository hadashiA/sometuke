XCODEROOT = %x[xcode-select -print-path].strip
SIM_SYSROOT = "#{XCODEROOT}/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator7.0.sdk/"
DEVICE_SYSROOT = "#{XCODEROOT}/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS7.0.sdk/"

MRuby::Build.new do |conf|
  toolchain :gcc

  conf.gembox 'default'
end

MRuby::CrossBuild.new('ios-simulator') do |conf|
  conf.bins = []

  conf.gembox 'default'

  conf.cc do |cc|
    cc.command = 'xcrun'
    cc.flags = %W(-sdk iphoneos clang -miphoneos-version-min=5.0 -arch i386 -isysroot #{SIM_SYSROOT} -g -O3 -Wall -Werror-implicit-function-declaration)
  end

  conf.linker do |linker|
    linker.command = 'xcrun'
    linker.flags = %W(-sdk iphoneos clang -miphoneos-version-min=5.0 -arch i386 -isysroot #{SIM_SYSROOT})
  end
end

MRuby::CrossBuild.new('ios-armv7') do |conf|
  conf.bins = []

  conf.gembox 'default'

  conf.cc do |cc|
    cc.command = 'xcrun'
    cc.flags = %W(-sdk iphoneos clang -arch armv7 -isysroot #{DEVICE_SYSROOT} -g -O3 -Wall -Werror-implicit-function-declaration)
  end

  conf.linker do |linker|
    linker.command = 'xcrun'
    linker.flags = %W(-sdk iphoneos clang -arch armv7 -isysroot #{DEVICE_SYSROOT})
  end
end

MRuby::CrossBuild.new('ios-armv7s') do |conf|
  conf.bins = []

  conf.gembox 'default'
  conf.cc do |cc|
    cc.command = 'xcrun'
    cc.flags = %W(-sdk iphoneos clang -arch armv7s -isysroot #{DEVICE_SYSROOT} -g -O3 -Wall -Werror-implicit-function-declaration)
  end

  conf.linker do |linker|
    linker.command = 'xcrun'
    linker.flags = %W(-sdk iphoneos clang -arch armv7s -isysroot #{DEVICE_SYSROOT})
  end
end

MRuby::CrossBuild.new('ios-arm64') do |conf|
  conf.bins = []

  conf.gembox 'default'
  conf.cc do |cc|
    cc.command = 'xcrun'
    cc.flags = %W(-sdk iphoneos clang -arch arm64 -isysroot #{DEVICE_SYSROOT} -g -O3 -Wall -Werror-implicit-function-declaration)
  end

  conf.linker do |linker|
    linker.command = 'xcrun'
    linker.flags = %W(-sdk iphoneos clang -arch arm64 -isysroot #{DEVICE_SYSROOT})
  end
end
