# backend-gateway-service
Containerized VSOME/IP service that directly communicates with the back-end server written in C++. Uses HTTP-Builder to generate HTTP requests and VSOME/IP-Client APIs to create the some-ip client.

## Where am I?
![diagram](./readme_imgs/diagram.png)


## build and push to container registry
```bash
$ docker buildx build --push \
--platform linux/amd64,linux/arm64 \
--tag registry.digitalocean.com/vehicle-plus/tcu_backend-gateway:v0 .
```

## how to run 
```
docker run --name backend_gateway -it --rm --privileged --net host -v docker run --name backend_gateway -it --rm --privileged --net host -v /tmp:/tmp:z registry.digitalocean.com/vehicle-plus/tcu_backend-gateway:v0
```
