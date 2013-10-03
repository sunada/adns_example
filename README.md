adns_example
============

testadns.c resolves IPv4 addresses. testadnsIPv6.c resolves IPv6 addresses. You input a host. Then it returns you an IP.

adns v1.2 can only resolve IPv4 addresses. adns v1.4 is able to resolve IPv6 addresses.

Here, I use adnsv1.4 to resolve IPv6 and IPv4 address.

Steps:

1. compile && build test_ipv6.c

    $ mv Makefile_test_ipv6 Makefile
    
    $ make
    
    $ ./test_ipv6

3. compile && build test_ipv4.c

    $ mv Makefile Makefile_test_ipv6

    $ mv Makefile_test_ipv4 Makefile

    $ make

    $ ./test_ipv4
    

Example:

$./test_ipv6

ipv6.google.com

ip: 2404:6800:4005:805::1012
