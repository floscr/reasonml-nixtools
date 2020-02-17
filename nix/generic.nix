{ pkgs, stdenv, ocamlPackages, gitignoreSource, static ? false }:

rec {
  nixtools = ocamlPackages.buildDune2Package {
    pname = "nixtools";
    version = "0.0.1-dev";

    src = gitignoreSource ./..;
    nativeBuildInputs = with ocamlPackages; [dune_2];
    propagatedBuildInputs = with ocamlPackages; [
      bigstringaf
    ];

    doCheck = false;

    meta = {
      description = "Client library for HTTP/1.X / HTTP/2 written entirely in OCaml.";
      license = stdenv.lib.licenses.bsd3;
    };
  };

  timer = stdenv.mkDerivation {
    name = "timer";
    version = "0.0.1-dev";

    src = gitignoreSource ./..;

    nativeBuildInputs = with ocamlPackages; [dune_2 ocaml findlib];

    buildPhase = ''
      echo "running ${if static then "static" else "release"} build"
      dune build src/Timer.exe --display=short --profile=${if static then "static" else "release"}
    '';
    installPhase = ''
      mkdir -p $out/bin
      mv _build/default/src/Timer.exe $out/bin/timer
    '';

    buildInputs = with ocamlPackages; [
      findlib
      nixtools
      cmdliner
      fmt
      camlzip
      ezgzip
      reason
    ];

    doCheck = false;

    meta = {
      description = "Timer tool";
      license = stdenv.lib.licenses.bsd3;
    };
  };
}

