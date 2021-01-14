# Setup
To compile the binaries you need a package called comidi.

For arch based systems:
```bash
yay -S comidilib
```
For debian based systems:
```bash
apt install libcomedi-dev
```

Then just compile like any other program
```bash
make
```
# Remote testing
You can upload your binary to the lab pc if you prefer using your own equipment.
- First of all you need to be connected to the NTNU's network (wifi). Either be at the campus or just use NTNU's VPN.
- And update the ip address in the makefile.
- `make remote`
You will be asked to fill the password for the lab pc. To save 1 second you can run `sshid-copy-id sudent@LAB_PC_IP`.
> Note: You will have to stop execution of the program your self.

> Note: If someone is testing their binaries as you deploy them then I honestly don't know what will happend.
