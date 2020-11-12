# Cryptfile

Encrypt or decrypt file

## Build

* Docker
```sh
docker build -t cryptfile -f docker/Dockerfile .
```

## Run

* Docker

*encrypt*

```sh
export ORIGIN_PB=$(pwd)/saved_model.pb

docker run \
  -v $ORIGIN_PB:/saved_model.pb \
  -v $PWD/output:/output \
  cryptfile \
  --mod enc \
  --input /saved_model.pb \
  --output /output/saved_model.en.pb

# output file: $PWD/output/saved_model.en.pb
```

*decrypt*

```sh
export ENCRYPT_PB=$(pwd)/output/saved_model.en.pb

docker run \
  -v $ENCRYPT_PB:/saved_model.en.pb \
  -v $PWD/output:/output \
  cryptfile \
  --mod dec \
  --input /output/saved_model.en.pb \
  --output /output/saved_model.pb

# output file: $PWD/output/saved_model.pb
```
