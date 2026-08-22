VOLUME_NAME=workspace
docker run --rm -v $PWD:/$VOLUME_NAME -w /$VOLUME_NAME -p 9000:9000 --name motion oven/bun bun $@
