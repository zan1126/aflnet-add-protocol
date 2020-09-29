# aflnet-add-protocol
Step 0: Compile and setup server and client
        git clone https://github.com/cloudflare/ngtcp2
        cd ngtcp2
        autoreconf -i
        ./configure PKG_CONFIG_PATH=$PWD/../openssl/build/lib/pkgconfig LDFLAGS="-Wl,-rpath,$PWD/../openssl/build/lib"
        make -j$(nproc) check

Step 1: Generate certificate and key
        cd examples 
        openssl genrsa -out server.key 2048 
        openssl req -new -x509 -key server.key -out server.crt -days 3650 

Step 2: Prepare seed inputs
        cd ngtcp2/examples
        //run the server
        ./server 127.0.0.1 4433 server.key server.crt
        sudo tcpdump -w ngtcp2.pcap -i lo port 4433
        //then run the client,and will get a file and get seed inputs
        ./client 127.0.0.1 4433 -i

Step 3: Fuzzing
        cd ngtcp2/examples
        afl-fuzz -d -i $ngtcp2/examples/in -o out -N udp://127.0.0.1/4433 -P NGTCP -D 10000 -q 3 -s 3 -E -K -R ./server 127.0.0.1 4433 server.key server.crt
