#!/bin/bash

echo "=== Media Center Setup ==="
echo ""

# Flask installieren
echo "[1/2] Installiere Flask..."
pip3 install flask --break-system-packages
echo "✓ Flask installiert"
echo ""

# Sudo Shutdown ohne Passwort
echo "[2/2] Konfiguriere sudo für Shutdown..."
echo "$USER ALL=(ALL) NOPASSWD: /sbin/shutdown" | sudo tee -a /etc/sudoers > /dev/null
echo "✓ Sudo Shutdown konfiguriert"
echo ""

# Autostart für Shutdown Server
echo "[3/3] Richte Autostart ein..."
mkdir -p ~/.config/autostart

cat > ~/.config/autostart/mediacenter-shutdown.desktop << EOF
[Desktop Entry]
Type=Application
Name=Mediacenter Shutdown Server
Exec=python3 $(pwd)/shutdown_server.py
Hidden=false
NoDisplay=false
X-GNOME-Autostart-enabled=true
EOF

cat > ~/.config/autostart/mediacenter-chrome.desktop << EOF
[Desktop Entry]
Type=Application
Name=Mediacenter Chrome
Exec=bash -c "sleep 5 && google-chrome --kiosk --noerrdialogs --disable-infobars --password-store=basic file://$(pwd)/index.html"
Hidden=false
NoDisplay=false
X-GNOME-Autostart-enabled=true
EOF

echo "✓ Autostart konfiguriert"


echo "=== Setup abgeschlossen! ==="
