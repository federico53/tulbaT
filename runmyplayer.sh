#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
JAVA_PATH="/usr/lib/jvm/java-21-openjdk-amd64/bin/java"  # Sostituisci con il percorso effettivo di Java 21
"$JAVA_PATH" -jar "$SCRIPT_DIR/Tabrut.jar" "$1" "$2" "$3"
