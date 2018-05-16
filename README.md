# hlds-won2-docker
Dockerfile for Half Life Dedicated Server

The Image will contain:
* Half Life Dedicated Server 3.1.1.1e
* Workaround for remote server/client crash
* WON2 Support
* Counter Strike 1.5 Mod
* Plug-in: AMX Mod 2010.1
* Plug-in: Podbot mm Full Pack V3B22

## Starting

### Build the image
``docker build -t hlds-won2 .``

### Create and start a new container
``docker run --name hlds-won2 -d -p 27015:27015 -p 27015:27015/udp hlds-won2``

### Stop the container
``docker container stop hlds-won2``

### Start stopped container
``docker container start hlds-won2``

### Remove stopped container
``docker container rm hlds-won2``


## Configuration

All files under _/config_ will be copied into the root directory of the Server.
Currently it contains basic configuration for the Counter Strike Server and the plug-ins
Additionally the Server can be configured with he following environment variables:

* __GAME__ Mod to load (-game), default cstrike
* __PORT__ Listen port (+port), default 27015
* __MAXPLAYERS__ Maximum players (+maxplayers), default 32
* __START_MAP__  Start map (+map), irequired for other games than cstrike, default de_dust2
* __RCON_PASSWORD__ Password for RCON (+rcon_password)
* __SERVERAME__ Server name (+hostname), will be overwritten by server.cfg, default HLDS WON2 Server

### Example: Start a TFC game server with server name "TFC Server" and the starting map well
``docker run --name hlds-won2 -d -p 27015:27015 -p 27015:27015/udp -e GAME=tfc -e START_MAP=well -e SERVERAME="TFC Server" hlds-won2``
