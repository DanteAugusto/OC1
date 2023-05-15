export SYSTEMC_HOME=/usr/local/systemc-2.3.3

systemc() {
  local TARGET_PATH="$(mktemp)"

  g++ \
    -I. -I"$SYSTEMC_HOME"/include \
    -L. -L"$SYSTEMC_HOME"/lib-linux64 \
    -Wl,-rpath="$SYSTEMC_HOME"/lib-linux64 \
    -o "$TARGET_PATH" "${@}" -lsystemc -lm \
    -Wall -Wextra

  if [ "$?" != 0 ]; then
      return
  fi

  "$TARGET_PATH"
}
