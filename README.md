# backend-gateway-service
Containerized service that directly communicate with the back-end server.

## Where am I?
![diagram](./readme_imgs/diagram.png)


## create network bridge
```
sudo docker network create --subnet=172.18.0.0/16 vsomeip_bridge
```

## how to run 
```
sudo docker run -it --net vsomeip_bridge --ip 172.18.0.3 -p 5001:5001/udp -v /tmp:/tmp:z udp_server_alpine
```