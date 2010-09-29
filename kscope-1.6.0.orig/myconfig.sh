./configure  --without-arts  --prefix=/opt/kscope 
make -j3 2>err
grep err err
