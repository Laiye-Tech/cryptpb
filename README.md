# Cryptfile
加解密文件

## Build

* 本机
```sh
bazel build --color=yes --verbose_failures --output_filter=DONT_MATCH_ANYTHING //cryptfile
# output binary is `./bazel-bin/cryptfile/cryptfile`
```

* Docker
```sh
docker build -t snowcrumble/cryptfile -f docker/Dockerfile .
```

## Run

* 本机

*加密*
```sh
./cryptfile -m enc -i saved_model.pb -o saved_model.en.pb -x saved_model.en.ex -e '2020-04-11 13:58:59'
```

*解密*
```sh
./cryptfile -m dec -i saved_model.en.pb -o saved_model.de.pb -x saved_model.en.ex
```

* Docker
*加密*
```sh
docker run \
  -v $ORIGIN_PB:/saved_model.pb \
  -v $PWD/output:/output \
  snowcrumble/cryptfile \
  --mod enc \
  --input /saved_model.pb \
  --output /output/saved_model.en.pb
```

*解密*
```sh
docker run \
  -v $ENCRYPT_PB:/saved_model.en.pb \
  -v $PWD/output:/output \
  snowcrumble/cryptfile \
  --mod dec \
  --input /saved_model.en.pb \
  --output /output/saved_model.pb
```
