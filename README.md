# Mediacenter

> Build a self-hosted media center on your computer or server.

Mediacenter is a self-hosted website tool designed to help you set up and run a private media hub on any PC or local server. By serving a localhost website, the application enables users to manage and access their media files locally through a web-based interface. It addresses the need for a straightforward, localized solution to organize and consume media without relying on external cloud providers.

---

## ✨ Features
- **🏠 Localhost Web Server Hosting** — Host and access your media center interface locally on any compatible computer or server.
- **💻 Command-Line Interface Utility** — Includes a dedicated CLI utility designed to support setup and system management tasks.
- **🛠️ Cross-Platform Server Deployment** — Engineered to deploy easily across diverse personal computing environments and home servers.

## 🎯 Use Cases
- Setting up a private media server on a home computer to host local content via a web interface.
- Using a CLI utility to configure and manage a locally hosted media application.

---

## ⚡ Installation & Setup

### 1. Clone the repository
```bash
git clone https://github.com/Flotschii-Code/Mediacenter.git
cd Mediacenter
```

### 2. Make commands global (Optional)
By default, you can run the CLI tool using `./mediacenter`. To make the `mediacenter` commands available globally from any directory, copy the binary to `/usr/local/bin/`:
```bash
sudo cp mediacenter /usr/local/bin/
```

---

## 🛠️ Usage & Commands

Once configured, use the following commands to manage your services and run your media center:

```bash
# Create and use a new custom service of your own
mediacenter --add [Name] [URL] [Logo]

# Enable and use an existing service from the databank
mediacenter --enable [Name]

# View all currently used services
mediacenter --services

# Force update the services.json file
mediacenter --update

# Start the localhost mediacenter server on port 8080 and update services.json
mediacenter --start
```

There are allready 66 services avialible by default. To see which are and which aren't please read the services_list.txt.

---

## 👥 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. Here is the standard workflow:

1. **Fork** the repository
2. **Clone** your fork:
   ```bash
   git clone https://github.com/Flotschii-Code/Mediacenter.git
   ```
3. **Branch**: Create your feature branch:
   ```bash
   git checkout -b feature/your-feature
   ```
4. **Commit**: Commit your changes:
   ```bash
   git commit -m 'feat: add some feature'
   ```
5. **Push**: Push to the branch:
   ```bash
   git push origin feature/your-feature
   ```
6. **Open** a Pull Request

*Please follow the existing code style and include tests for new behavior where applicable.*

---
