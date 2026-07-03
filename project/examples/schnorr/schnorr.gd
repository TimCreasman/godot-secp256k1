extends Node2D

var _secp256k1 := Secp256k1.new()

signal errored(err: String)

func _init():
	errored.connect(_on_errored)

func _on_generate_key_button_button_down() -> void:
	_clear_all_labels()
	$VBoxContainer/GridContainer/PublicKey.clear()
	$VBoxContainer/GridContainer/PrivateKey.clear()

	# Must be called before signing anything
	var err = _secp256k1.keygen()
	if err != Error.OK:
		errored.emit(error_string(err))
		return

	var pubkey = _secp256k1.get_public_key()

	$VBoxContainer/GridContainer/PublicKey.text = pubkey.hex_encode()
	$VBoxContainer/GridContainer/PrivateKey.text = "Private key set"

func _on_schnorr_sign_button_button_down() -> void:
	_clear_all_labels()
	var message_to_sign = $VBoxContainer/SignMessageContainer/MessageToSign.text

	# Typically a schnorr signature takes in a 32 byte hash of the message text
	var signed_bytes = _secp256k1.schnorr_sign(message_to_sign.sha256_buffer())

	# An empty sign is a "sign" something went wrong
	if signed_bytes.is_empty():
		errored.emit("Something went wrong, check the logs")
		return

	$VBoxContainer/SignMessageContainer/SchnorrSignature.text = signed_bytes.hex_encode()

func _on_verify_schnorr_sign_button_button_down() -> void:
	_clear_all_labels()

	var signature_to_verify = $VBoxContainer/VerifySignageContainer/SignatureToVerify.text as String
	var message_to_check_against = $VBoxContainer/VerifySignageContainer/MessageToVerify.text as String

	var err = _secp256k1.schnorrsig_verify(message_to_check_against.sha256_buffer(), signature_to_verify.hex_decode())
	if err != Error.OK:
		errored.emit(error_string(err))
		return

	$VBoxContainer/VerifySignageContainer/HBoxContainer/SchnorrSignatureValidResult.text = "Your signature is valid and matches the provided message!"

func _on_errored(err: String):
	$VBoxContainer/ErrorLabel.text = err

func _clear_all_labels():
	$VBoxContainer/ErrorLabel.text = ""
	$VBoxContainer/VerifySignageContainer/HBoxContainer/SchnorrSignatureValidResult.text = ""
