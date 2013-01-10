all: cert-0.crt

rel3_dodroot_2048.cac:
	wget -O "$@.new" http://dodpki.c3pki.chamb.disa.mil/rel3_dodroot_2048.cac
	mv "$@.new" "$@"

cert-%.crt: rel3_dodroot_2048.cac
	idx=0; \
	( \
		openssl pkcs7 -in rel3_dodroot_2048.cac -inform DER -print_certs -text; \
	) | while IFS='' read -r line; do \
		if [ -z "$${line}" ]; then \
			continue; \
		fi; \
		echo "$${line}" >> "cert-$${idx}.crt"; \
		if [ "$${line}" == "-----END CERTIFICATE-----" ]; then \
			idx=$$[$$idx + 1]; \
		fi; \
	done

clean:
	rm -f cert-*.crt
	rm -f rel3_dodroot_2048.cac.new

distclean: clean
	rm -f rel3_dodroot_2048.cac
