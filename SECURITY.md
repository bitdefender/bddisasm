# Handling Security Issues Discovered in the bddisasm Related Projects

bddisasm contains several complex projects (x86/x64 disassembler, instruction emulator) 
and complexity can lead to potential bugs. In this regard, we advise
researchers to follow responsible disclosure and notify us before publicly 
disclosing any security issue, to give us the opportunity to fix it.

## How to Report a Security Issue

Identified security issues must be properly described, and steps of reproduction
must be provided. Proof of concepts are strongly encouraged, but not strictly
required, as long as the reproduction steps are clear, and we are able to 
reproduce the issue. Please make sure to include the following:

* Brief description of the problem
* Steps of reproduction
* Core dumps, memory dumps and any other useful information that can be used to triage and fix the issue
* Impact

## Where to Report a Security Issue

Our security email address is `hvmi-security@bitdefender.com`. Please use the
following PGP key to encrypt your e-mail, and make sure to include your public 
key, in case we need to contact you for more info:

```
-----BEGIN PGP PUBLIC KEY BLOCK-----
Version: GnuPG

mQENBFUgwRUBCADcIpqNwyYc5UmY/tpx1sF/rQ3knR1YNXYZThzFV+Gmqhp1fDH5
qBs9foh1xwI6O7knWmQngnf/nBumI3x6xj7PuOdEZUh2FwCG/VWnglW8rKmoHzHA
ivjiu9SLnPIPAgHSHeh2XD7q3Ndm3nenbjAiRFNl2iXcwA2cTQp9Mmfw9vVcw0G0
z1o0G3s8cC8ZS6flFySIervvfSRWj7A1acI5eE3+AH/qXJRdEJ+9J8OB65p1JMfk
6+fWgOB1XZxMpz70S0rW6IX38WDSRhEK2fXyZJAJjyt+YGuzjZySNSoQR/V6vNYn
syrNPCJ2i5CgZQxAkyBBcr7koV9RIhPRzct/ABEBAAG0IVNlY3VyaXR5IDxzZWN1
cml0eUBwYXJhZ29uaWUuY29tPokBOQQTAQIAIwUCVSDBFQIbAwcLCQgHAwIBBhUI
AgkKCwQWAgMBAh4BAheAAAoJEGuXocKCZATat2YIAIoejNFEQ2c1iaOEtSuB7Pn/
WLbsDsHNLDKOV+UnfaCjv/vL7D+5NMChFCi2frde/NQb2TsjqmIH+V+XbnJtlrXD
Vj7yvMVal+Jqjwj7v4eOEWcKVcFZk+9cfUgh7t92T2BMX58RpgZF0IQZ6Z1R3FfC
9Ub4X6ykW+te1q0/4CoRycniwmlQi6iGSr99LQ5pfJq2Qlmz/luTZ0UX0h575T7d
cp2T1sX/zFRk/fHeANWSksipdDBjAXR7NMnYZgw2HghEdFk/xRDY7K1NRWNZBf05
WrMHmh6AIVJiWZvI175URxEe268hh+wThBhXQHMhFNJM1qPIuzb4WogxM3UUD7m5
AQ0EVSDBFQEIALNkpzSuJsHAHh79sc0AYWztdUe2MzyofQbbOnOCpWZebYsC3EXU
335fIg59k0m6f+O7GmEZzzIv5v0i99GS1R8CJm6FvhGqtH8ZqmOGbc71WdJSiNVE
0kpQoJlVzRbig6ZyyjzrggbM1eh5OXOk5pw4+23FFEdw7JWU0HJS2o71r1hwp05Z
vy21kcUEobz/WWQQyGS0Neo7PJn+9KS6wOxXul/UE0jct/5f7KLMdWMJ1VgniQmm
hjvkHLPSICteqCI04RfcmMseW9gueHQXeUu1SNIvsWa2MhxjeBej3pDnrZWszKwy
gF45GO9/v4tkIXNMy5J1AtOyRgQ3IUMqp8EAEQEAAYkBHwQYAQIACQUCVSDBFQIb
DAAKCRBrl6HCgmQE2jnIB/4/xFz8InpM7eybnBOAir3uGcYfs3DOmaKn7qWVtGzv
rKpQPYnVtlU2i6Z5UO4c4jDLT/8Xm1UDz3Lxvqt4xCaDwJvBZexU5BMK8l5DvOzH
6o6P2L1UDu6BvmPXpVZz7/qUhOnyf8VQg/dAtYF4/ax19giNUpI5j5o5mX5w80Rx
qSXV9NdSL4fdjeG1g/xXv2luhoV53T1bsycI3wjk/x5tV+M2KVhZBvvuOm/zhJje
oLWp0saaESkGXIXqurj6gZoujJvSvzl0n9F9VwqMEizDUfrXgtD1siQGhP0sVC6q
ha+F/SAEJ0jEquM4TfKWWU2S5V5vgPPpIQSYRnhQW4b1
=xJPW
-----END PGP PUBLIC KEY BLOCK-----
```

## Rewarding the Security Researchers

While there are no bounties offered for issues discovered in this project, 
we do credit all the security researchers who contribute to making the 
project safer.