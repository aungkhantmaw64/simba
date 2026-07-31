IMG_NAME=simba-ci

docker-install-dev:
	docker build -t ${IMG_NAME} --progress plain .

docker-dev-cli:
	docker container run \
		--rm -it \
		--privileged \
		-v ${PWD}:/app \
		${IMG_NAME} \
	 bash
