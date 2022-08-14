# Table of Contents  
- [Table of Contents](#table-of-contents)
- [1. Update & upgrade, install and remove](#1-update--upgrade-install-and-remove)
  - [Update & upgrade](#update--upgrade)
  - [Install](#install)
    - [Debian:](#debian)
    - [Arch:](#arch)
  - [Delete](#delete)
    - [Debian:](#debian-1)
    - [Arch:](#arch-1)
- [2. Set up SSH](#2-set-up-ssh)
  - [Install openSSH](#install-openssh)
  - [Enable and start service](#enable-and-start-service)
  - [Configure (optional)](#configure-optional)
- [3. Set up Firewall](#3-set-up-firewall)
  - [Install UFW](#install-ufw)
  - [Configure UFW](#configure-ufw)
- [4. MFA for SSH (Google Authenticator)](#4-mfa-for-ssh-google-authenticator)
  - [Install](#install-1)
  - [Options](#options)
  - [Pair with ssh](#pair-with-ssh)
- [5. Send email upon SSH login](#5-send-email-upon-ssh-login)
  - [Install](#install-2)
  - [Configure](#configure)
  - [Bashrc](#bashrc)
- [6. Technitium DNS server on linux](#6-technitium-dns-server-on-linux)
  - [Install](#install-3)
  - [Configure](#configure-1)
- [7. OpenVPN on linux](#7-openvpn-on-linux)
  - [Dynamic DNS](#dynamic-dns)
  - [Install](#install-4)
  - [Configure Server and launch service](#configure-server-and-launch-service)
  - [Clients](#clients)
- [Miscellaneous](#miscellaneous)
  - [Time wrong (Arch)](#time-wrong-arch)


# 1. Update & upgrade, install and remove
## Update & upgrade
Debian:
~~~bash
sudo apt-get update
sudo apt-get upgrade
~~~

Arch:
~~~bash
sudo pacman -Syu
~~~
## Install
### Debian:
~~~bash
sudo apt-get install <package name>
~~~

### Arch:
~~~bash
sudo pacman -S <package name>
~~~
On arch you also may want to enable using the AUR. Make sure you have git:
~~~bash
sudo pacman -S git base-devel
~~~
Then install the package
~~~bash
git clone https://aur.archlinux.org/<pkgName>.git
cd <pkgName>/
makepkg -sri
~~~
## Delete
### Debian:
~~~bash
sudo apt-get --purge remove <package name>
~~~

### Arch:
~~~bash
sudo pacman -Rn <package name>
~~~
And for an AUR installed package you can still remove with pacman.

# 2. Set up SSH
## Install openSSH
Debian:
~~~bash
sudo apt-get install openssh-server
~~~

Arch:
~~~bash
sudo pacman -Sy openssh
~~~
## Enable and start service
~~~bash
sudo systemctl enable sshd
sudo systemctl start sshd
systemctl status sshd
~~~
## Configure (optional)
Using a text editor (i.e. nano, vim..) edit the **/etc/sshsshd_config** file:
~~~bash
Port 22 
PermitRootLogin no
X11Forwarding yes
PubkeyAuthentication yes
~~~
1. Default port is 22, set to something else for more security
2. Disallows login with root
3. Lets remote computers push interactive windows to your local computer over a network
4. Only devices with authorized ssh keys can ssh in.

Navigate to the **~/.ssh/authorized_keys** and **copy in the ssh key** of your local device** called **id_rsa.pub**:
~~~bash
mkdir ~/.ssh
cd ~/.ssh
touch authorized_keys
nano authorized_keys
~~~
Restart to apply changes
~~~bash
sudo systemctl restart sshd
~~~
To ssh into the device:
~~~
ssh -p <port #> <username>@<ip>
~~~
# 3. Set up Firewall
## Install UFW
Debian:
~~~bash
sudo apt-get apt-get install ufw
~~~

Arch:
~~~bash
sudo pacman -Sy ufw
~~~
## Configure UFW
~~~bash
sudo systemctl enable ufw
sudo systemctl start ufw
sudo ufw default deny incoming
sudo ufw default allow outgoing
sudo ufw allow <ssh port number>
sudo ufw delete <port number>
sudo ufw enable
sudo ufw status
~~~
# 4. MFA for SSH (Google Authenticator) 
## Install
Arch:
~~~bash
sudo pacman -S libpam-google-authenticator
google-authenticator
~~~
## Options
1. Do you want authentication tokens to be time-based?

    <font color="green"> y </font>
2. Use QR code and confirm code. If the code doesn't work check out *Wrong Time* in **Mischelaneous** further down.
3. Do you want me to update your "/home/zackkomo/.google_authenticator" file?

    <font color="green"> y </font>
4. Do you want to disallow multiple uses of the same authentication
token?
<font color="red"> n </font>
5. By default, a new token is generated every 30 seconds .. This will permit for a time skew of up to 4 minutes
between client and server.

    <font color="red"> n </font>

6. If the computer that you are logging into isn't hardened against brute-force...

    <font color="red"> n </font>

## Pair with ssh
Next run this to make ssh use google-auth. 
~~~bash
echo "auth required pam_google_authenticator.so" | sudo tee -a /etc/pam.d/sshd
~~~
And set MFA on for ssh in **/etc/ssh/sshd_config** and restart:
~~~bash
echo "ChallengeResponseAuthentication yes" | sudo tee -a /etc/ssh/sshd_config
echo "AuthenticationMethods publickey,password publickey,keyboard-interactive" | sudo tee -a /etc/ssh/sshd_config
sudo systemctl restart sshd.service
~~~
# 5. Send email upon SSH login 
## Install
Arch:
~~~bash
sudo pacman -Sy sendmail
sudo pacman -Sy postfix
~~~
Start Services
~~~bash
sudo systemctl start postfix
sudo systemctl enable postfix
~~~
## Configure
Point postfix to gmail:

~~~bash
echo "relayhost = [smtp.gmail.com]:587" | sudo tee -a /etc/postfix/main.cf
echo "smtp_sasl_auth_enable = yes" | sudo tee -a /etc/postfix/main.cf
echo "smtp_sasl_password_maps = hash:/etc/postfix/sasl_passwd" | sudo tee -a /etc/postfix/main.cf
echo "smtp_sasl_security_options = noanonymous" | sudo tee -a /etc/postfix/main.cf
echo "smtp_tls_CAfile = /etc/ssl/certs/ca-certificates.crt" | sudo tee -a /etc/postfix/main.cf
echo "smtp_use_tls = yes" | sudo tee -a /etc/postfix/main.cf
~~~
Save gmail to use in :
~~~bash
sudo nano /etc/postfix/sasl_passwd
~~~
by typing:
~~~
[smtp.gmail.com]:587    <username>@gmail.com:<password>
~~~
and then changing permissions.
~~~bash
sudo chmod 400 /etc/postfix/sasl_passwd
sudo postmap /etc/postfix/sasl_passwd
systemctl restart postfix
~~~
## Bashrc
Then in your **.bashrc** file add:
~~~bash
if [ -n "$SSH_CLIENT" ]; then
    TEXT="$(date): ssh login to ${USER}@$(hostname -f)"
    TEXT="$TEXT from $(echo $SSH_CLIENT|awk '{print $1}')"
    echo $TEXT|mail -s <SUBJECT> <RECIPIENT EMAIL>
fi
~~~
# 6. Technitium DNS server on linux
## Install
~~~bash
curl -sSL https://download.technitium.com/dns/install.sh | sudo bash
sudo ufw allow 5380
sudo ufw allow 53
~~~
## Configure
Insert custom blocklist here
~~~bash
sudo touch /etc/dns/config/blocklists/blocklist.txt
~~~
and go to **[servername]**:5380 to configure through a browser.


# 7. OpenVPN on linux

1. Make sure the device running OpenVPN has a static IP.
2. Make sure the router in the network is forwarding to the right port.
3. Make sure the router's network is given a static IP from the ISP or that a dynamic DNS service is set up.

## Dynamic DNS
ISP's will switch out your IP address every couple of weeks. This means that if you plan on VPN-ing in using your IP, it may change on you. Look up a free DDNS provider like [dynu.com](https://www.dynu.com/) and set up an account. Then on the server do the following:
~~~bash
sudo EDITOR=nano crontab -e

*/15 * * * * wget -O dynulog -4 "https://api.dynu.com/nic/update?hostname=example.dynu.com&myip=10.0.0.0&myipv6=no&username=someusername&password=somepassword"
~~~
and replace "example.dynu.com" with your hostname and "someusername" and "somepassword" with the username and password. For the password use the hashed version using [this](https://www.dynu.com/NetworkTools/Hash) and use the MD5 hash.

To make sure it worked, copy and paste the wget portion and onward in the temrinal and cat out the log. You should get "no change" or "good" followed by the new IP.

## Install
~~~bash
pacman -S openvpn curl
sudo ufw allow 1194
curl -O https://raw.githubusercontent.com/Angristan/openvpn-install/master/openvpn-install.sh
chmod +x openvpn-install.sh
sudo ./openvpn-install.sh
~~~
## Configure Server and launch service

Provide the client name the VPN should expect and encrypt with a password. Keep everything else default UNLESS:
1. you want to use your own DNS server, switch that)
2. you are using DDNS, provide the DDNS URL where it asks for the IP 
At the end it should generate a .ovpn file. Cat it and make sure the line starting with "remote" has your DDNS hostname.

Then, if the service isn't running and you get permission errors, do the following:
~~~bash
sudo chown -R openvpn:network /etc/openvpn/ /var/log/openvpn/
sudo systemctl start openvpn-server@server.service
sudo systemctl status openvpn-server@server.service
~~~
 ## Clients

 Install OpenVPN Client on your devices and use the .ovpn file from the server to login.

# Miscellaneous
## Time wrong (Arch)
Use **sudo timedatectl status** to make sure your time zone is correct. Then:
~~~bash
sudo systemctl enable --now systemd-timesyncd.service
sudo timedatectl set-ntp true
~~~
