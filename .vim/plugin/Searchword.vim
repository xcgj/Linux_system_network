" au! QuickFixCmdPre *.[ch] call Searchword()
func Searchword()
  let w = expand("<cword>")             " 在当前光标位置抓词
  exe "vimgrep " w " **/*.c **/*.h **/*.py"
  exe 'copen'
endfun
