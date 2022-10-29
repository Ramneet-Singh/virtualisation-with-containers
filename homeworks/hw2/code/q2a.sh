mkdir new-root
cp --verbose --parents /bin/bash new-root/
# source for following 4 lines - https://github.com/Nerdmind/Snippets/blob/master/Bash/ldd-copy-dependencies.sh
for library in $(ldd "/bin/bash" | cut -d '>' -f 2 | awk '{print $1}')
do
	[ -f "${library}" ] && cp --verbose --parents "${library}" "new-root/"
done

# write test-cmds.sh for testing the required commands
cat >new-root/test-cmds.sh <<'EOF'
for cmd in 'cd' 'cd .' 'pwd' 'less'
do
  if ${cmd} ; then
    echo "${cmd} works!"
  else
    echo "${cmd} doesn't work!"
  fi
done
EOF
sudo chroot new-root bash "./test-cmds.sh"
