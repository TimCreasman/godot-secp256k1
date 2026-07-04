# v1.0.0

> A godot proxy into the high-performance high-assurance C library for digital signatures and other cryptographic primitives on the secp256k1 elliptic curve.

**NOTE:** This currently only supports the Schnorr signature methods, and not the full secp256k1 library.
The current state of the library was to support Nostr relay's in this repository:
Feel free to contribute and expand the API surface.

---

## Features
*  Schnorr signatures according to [BIP-340](https://github.com/bitcoin/bips/blob/master/bip-0340.mediawiki).

---

## Installation

1. Copy everything under `addons/` into your Godot project.

---

## Usage

Example scenes are available under `project/examples`.

### Schnorr signing
> See `projects/examples/schnorr.gd` for further examples.

```gdscript
# Generate a new Secp256k1 instance
var _secp256k1 := Secp256k1.new()
# Generate the private / public keys. Returns the public key.
var pubkey = _secp256k1.get_public_key()

var message = "Sign me"
# NOTE: the parameter must be a sha256_buffer as schnorr signatures take in a 32 byte hash of the message text
var signed_bytes = _secp256k1.schnorr_sign(message.sha256_buffer())

# Verify an existing signature:
var err = _secp256k1.schnorrsig_verify(message.sha256_buffer(), signed_bytes.hex_decode())
```
