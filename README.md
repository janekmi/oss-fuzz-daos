# OSS-Fuzz for DAOS

[![Main](https://github.com/janekmi/oss-fuzz-daos/actions/workflows/main.yaml/badge.svg)](https://github.com/janekmi/oss-fuzz-daos/actions/workflows/main.yaml)

Test DAOS with OSS-Fuzz.

## Glossary

This repository follows glossary recommended by OSS-Fuzz project and defined by Google [here](https://github.com/google/fuzzing/blob/master/docs/glossary.md).

## Manual build

OSS-Fuzz builds targets in a Docker container. Please see [main.yaml](.github/workflows/main.yaml) workflow to see an example.

But it is also possible to build DAOS targets directly on the host. To do this you need both libprotobuf-mutator and DAOS repositories cloned locally e.g.

```sh
cd ~
mkdir google
cd google
git clone --depth 1 --branch v1.5 https://github.com/google/libprotobuf-mutator.git
cd -
mkdir daos-stack
cd daos-stack
git clone --depth 1 --branch janekmi/OSS-Fuzz-ready https://github.com/daos-stack/daos.git
cd -
```

**Note**: The version of DAOS currently available on official branches do NOT contains all necessary adjustments to build DAOS targets without errors. Please use the branch provided above.

Having that in place you can build DAOS targets as follows:

```sh
cd projects/daos
LPM_SRC_DIR=~/google/libprotobuf-mutator/ DAOS_SRC_DIR=~/daos-stack/daos/src/ ./build.sh
```

## Basic usage

When the targets are build you can just run them as follows:

```sh
cd projects/daos
./targets/target_btree
# ...
artifact_prefix='./'; Test unit written to ./crash-463fad88b393a04e355dfab23e090639651f3946
# ...
```

When the run stops it writes down a scenario which crashed (or stopped for other reasons) in a file as above: `crash-*`. To reproduce or debug you may run the failed scenario directly:

```sh
./targets/target_btree ./crash-463fad88b393a04e355dfab23e090639651f3946
# ...
./targets/target_btree: Running 1 inputs 1 time(s) each.
Running: crash-463fad88b393a04e355dfab23e090639651f3946
# ...
```

**Note**: `crash-*` files (and other input dumps produced by targets) are human-readable. Which makes understanding the crash scenario relatively easy e.g.

```capnp
commands {
  create {
    feat_uint_key: true
    feat_embed_first: true
  }
}
commands {
  update {
    entries_num: 32768
  }
}
commands {
  drain {
    credits_num: 32
  }
}
commands {
  delete {
    entries_num: 8
  }
}
```

## Command line options

All command line options are listed in the help message:

```sh
./target_btree -help=1
```

## Advanced usage

To maintain minute control over the environment the OSS-Fuzz employes Docker images to both build and run targets. As far as, you can start with [basic usage](#basic-usage) some crashes, leaks etc. may be reproducible only in exactly the same environment.

### Prepare repositories

This repository intentionally follows the [google/oss-fuzz](/google/oss-fuzz) repository's directory structure but it does not clone all its contents to keep it lean and DAOS-focused. When you want to investigate more advanced issues you need to combine both this repository and [google/oss-fuzz](/google/oss-fuzz) as follows.

```sh
cd ~
mkdir google
cd google
git clone https://github.com/google/oss-fuzz
cd oss-fuzz
# create a symbolic link for the projects/daos directory in this repository e.g.
ln -s ~/daos/oss-fuzz-daos/projects/daos projects/daos
```

### Build image

```sh
python3 infra/helper.py build_image --no-pull daos
```

### Build targets

Available sanitizers e.g. address, undefined, ...

```sh
python3 infra/helper.py build_fuzzers --sanitizer address daos
```

### Reproduce a problem

**Note**: Make sure you compiled your targets with the sanitizer which discovered the problem.

```sh
python3 infra/helper.py reproduce daos target_btree projects/daos/targets/crash-d2e5292aa65534ab341793c248d1c1a5ac173201
```

## External resources

- https://google.github.io/oss-fuzz/
- https://llvm.org/docs/LibFuzzer.html
- https://github.com/google/fuzzing
- https://google.github.io/clusterfuzzlite/
- https://github.com/google/libprotobuf-mutator
