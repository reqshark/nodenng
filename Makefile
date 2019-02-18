.PHONY: clean debug check test publish

ifeq ($(shell uname -s), Darwin)
  sed=sed -i ''
else
  sed=sed -i
endif

ALL:
	npm i
	@npm i nanomsg; $(sed) '/nanomsg/d' package.json; rm package-l*

publish:
	rm -rf build deps nng node_modules package-*
	npm publish

check:
	npm t

test:
	npm t

clean:
	rm -fr package-* node_modules build nng deps
