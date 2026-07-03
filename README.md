# godot-secp256k1

> A godot proxy into the high-performance high-assurance C library for digital signatures and other cryptographic primitives on the secp256k1 elliptic curve.

![godot-piggyback logo](assets/icon.svg)

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

