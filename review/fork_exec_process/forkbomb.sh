# You probably need to be root to run this
lxd init --auto
lxc launch ubuntu:16.04 forkbomb

# Limit resource usage so that we don't lock up the host machine
lxc config set forkbomb limits.cpu.allowance 5%
lxc config set forkbomb limits.memory 100MB

# Execute forkbomb script in container
# lxc exec forkbomb -- bash -c ":(){ : | : & }; :"
# This doesn't work becuase lxc just exits since we're not running
# interactively.

echo "Enter the following line of code:"
echo ":(){ : | :& }; :"
echo "Warning: This will hog system resources and lock up your terminal until killed"
exec lxc exec forkbomb bash

# To regain control of your resources run:
# lxc stop -f forkbomb
