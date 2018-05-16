FROM i386/ubuntu:16.04

# Install dependencies
RUN apt-get update && apt-get install -y \
  unzip \
  wget

#Create service user and use it
RUN groupadd -r hlds && useradd --no-log-init --system --create-home --home-dir /hlds_l --gid hlds  hlds
USER hlds

# install HLDS Linux 3.1.1.1e & Counter-Strike 1.5, don't change the order
RUN wget -q -O -  http://dl.4players.de/f0/4players/halflife/server/linux/hlds_l_3111_full.bin | \
  tail -c+8338 | head -c121907818 | \
  tar -xzf - -C /
RUN wget -q -O - http://dl.4players.de/f0/4players/halflife/mods/cstrike/server/linux/cs_15_full.tar.gz | \
  tar -xzf - -C /hlds_l
RUN wget -q -O - http://dl.4players.de/f0/4players/halflife/server/linux/hlds_l_3111e_update.tar.gz | \
  tar -xzf - -C /

# install AMX Mod 2010.1 and Podbot mm Full Pack V3B22
RUN wget -q -O /tmp/amxmod_2010.1_cs-en.zip http://www.amxmod.net/amxfiles/amxmod_2010.1/amxmod_2010.1_cs-en.zip && \
  unzip -q -d /hlds_l/cstrike /tmp/amxmod_2010.1_cs-en.zip && \
  rm -f /tmp/amxmod_2010.1_cs-en.zip
RUN wget -q -O /tmp/podbot_full_V3B22.zip ftp://77.120.162.198/public/games/servers/hlds/mods/bots/podbot_full_V3B22.zip && \
  unzip -q -d /hlds_l/cstrike/addons /tmp/podbot_full_V3B22.zip && \
  rm -f /tmp/podbot_full_V3B22.zip

# WON2 stuff and fixes
COPY hlds/valvecomm.lst /hlds_l/valve/valvecomm.lst
COPY hlds/*.so  /hlds_l/
COPY hlds/hlds_run /hlds_l/hlds_run

# deploy config directory containing server and addon config files
COPY configs/ /hlds_l/

# Set terminal emulation
ENV TERM xterm

# expose ports
# Game data & RCON
EXPOSE 27015/udp 27015

# Run the half life dedicated server
WORKDIR /hlds_l/
CMD ["./hlds_run"]
