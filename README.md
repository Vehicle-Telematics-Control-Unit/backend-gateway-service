# backend-gateway-service
Containerized service that directly communicate with the back-end server.

## Where am I?
![diagram](./readme_imgs/diagram.png)



## how to run 
```
docker run -it --privileged --net host -v /tmp:/tmp:z backend_gateway_alpine
```