all: cert-0.crt

rel3_dodroot_2048.cac:
	wget -O Certificates_PKCS7_v4.1_DoD.zip http://iasecontent.disa.mil/pki-pke/Certificates_PKCS7_v4.1_DoD.zip
	unzip Certificates_PKCS7_v4.1_DoD.zip Certificates_PKCS7_v4.1_DoD/Certificates_PKCS7_v4.1_DoD.der.p7b
	mv Certificates_PKCS7_v4.1_DoD/Certificates_PKCS7_v4.1_DoD.der.p7b "$@"
	rm -rf Certificates_PKCS7_v4.1_DoD Certificates_PKCS7_v4.1_DoD.zip

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
