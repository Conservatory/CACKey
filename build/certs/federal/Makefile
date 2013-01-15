all: cert-1.crt CPCA_TRCA.crt
	grep -l 'Issuer: C=US, O=U.S. Government, OU=FPKI, CN=Federal Bridge CA' *.crt | xargs rm -f
	grep -l 'Subject: C=US, O=U.S. Government, OU=FPKI, CN=Federal Common Policy CA' *.crt | xargs grep -l 'Issuer: C=US, O=U.S. Government, OU=Department of the Treasury, OU=Certification Authorities, OU=US Treasury Root CA' | xargs rm -f

CPCA_TRCA.crt:
	wget -O - --no-check-certificate https://pki.treas.gov/CPCA_TRCA.cer | openssl x509 -text > "$@.new"
	mv "$@.new" "$@"

caCertsIssuedTofcpca.p7c:
	wget -O "$@.new" http://http.fpki.gov/fcpca/caCertsIssuedTofcpca.p7c
	mv "$@.new" "$@"

root_sia.p7b:
	wget -O "$@.new" --no-check-certificate https://pki.treas.gov/root_sia.p7b
	mv "$@.new" "$@"

cert-%.crt: root_sia.p7b caCertsIssuedTofcpca.p7c
	idx=0; \
	( \
		openssl pkcs7 -in root_sia.p7b -inform DER -print_certs -text; \
		openssl pkcs7 -in caCertsIssuedTofcpca.p7c -inform DER -print_certs -text; \
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
	rm -f CPCA_TRCA.crt.new root_sia.p7b.new caCertsIssuedTofcpca.p7c.new

distclean: clean
	rm -f CPCA_TRCA.crt root_sia.p7b caCertsIssuedTofcpca.p7c
