openssl genrsa 1024 > host.key
openssl req -new -x509 -nodes -sha1 -days 365 -key host.key > host.cert
cat host.cert host.key > host.pem

