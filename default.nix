{ stdenv, cmake, gnumake, gcc-arm-embedded, meson, ninja, bash
, buildtype ? "debug", lib }:

assert buildtype == "debug" || buildtype == "release";

stdenv.mkDerivation rec {
  inherit buildtype;

  pname = "firmware";
  version = lib.fileContents ./VERSION;
  src = ./.;

  # order of ninja+meson nad cmake+gnumake will impact which generator is chosen
  buildInputs = [ gcc-arm-embedded ninja meson cmake gnumake ];

  dontFixup =
    true; # if you use fixupPhase (do something after build), remove this
  dontStrip = true;
  dontPatchELF = true;

  # Firmware/device info
  device = "STM32G031K8";
  binary = "${pname}-${buildtype}-${version}.bin";
  executable = "${pname}-${buildtype}-${version}.elf";

  # cmake
  cmakeFlags = [
    "-DPROJECT_VERSION=${version}"
    "-DCMAKE_BUILD_TYPE=${buildtype}"
    "-DDUMP_ASM=OFF"
  ];

  # meson
  mesonBuildType = "${lib.strings.toLower buildtype}";
  mesonFlags = [
    "--cross-file=gcc-arm-none-eabi.meson"
    "--cross-file=stm32g0.meson"
    "--buildtype=${buildtype}"
  ];

  patchPhase = ''
    patchShebangs glob.sh
  '';

  # "save" outputs
  installPhase = ''
    mkdir -p $out/bin
    cp *.bin *.elf *.s $out/bin
    cp compile_commands.json $out
  '';
}
