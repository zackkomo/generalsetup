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
Debian:
~~~bash
sudo apt-get install <package name>
~~~

Arch:
~~~bash
sudo pacman -S <package name>
~~~

## Delete
Debian:
~~~bash
sudo apt-get --purge remove <package name>
~~~

Arch:
~~~bash
sudo pacman -Rn <package name>
~~~
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
Arch:
~~~bash
sudo pacman -S libpam-google-authenticator
google-authenticator
~~~
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
# 4. Send email upon SSH login 
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
Then in your **.bashrc** file add:
~~~bash
if [ -n "$SSH_CLIENT" ]; then
    TEXT="$(date): ssh login to ${USER}@$(hostname -f)"
    TEXT="$TEXT from $(echo $SSH_CLIENT|awk '{print $1}')"
    echo $TEXT|mail -s <SUBJECT> <RECIPIENT EMAIL>
fi
~~~

# Miscellaneous
## Time wrong (Arch)
Use **sudo timedatectl status** to make sure your time zone is correct. Then:
~~~bash
sudo systemctl enable --now systemd-timesyncd.service
sudo timedatectl set-ntp true
~~~
