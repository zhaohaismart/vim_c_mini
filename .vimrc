set nu
set nocompatible
set backspace=indent,eol,start

call plug#begin()
Plug 'scrooloose/nerdtree', {'on' : 'NERDTreeToggle'}
Plug 'vim-airline/vim-airline'           " 美化状态栏
Plug 'vim-airline/vim-airline-themes'    " 设置ailine
Plug 'plasticboy/vim-markdown'           " markdown高亮
Plug 'octol/vim-cpp-enhanced-highlight'  " C++代码高亮
Plug 'mhinz/vim-signify'
Plug 'dense-analysis/ale'
Plug 'morhetz/gruvbox'					 " 主题
Plug 'mileszs/ack.vim'
Plug 'easymotion/vim-easymotion'
Plug 'tomasr/molokai'
Plug 'altercation/vim-colors-solarized'
let g:plug_timeout = 300
Plug 'ycm-core/YouCompleteMe', {'do' : './install.py'}
call plug#end()

"colorscheme molokai
"let g:molokai_original = 1
"let g:rehash256 = 1
"

"syntax enable
"set background=dark
"colorscheme solarized

"< F5> 编译和运行C++
map <F5> :call CompileRunGpp()<CR>
func! CompileRunGpp()
exec "w"
exec "!clang++ % -o %<"
exec "! ./%<"
endfunc




"让vimrc配置变更立即生效'
autocmd BufWritePost $MYVIMRC source $MYVIMRC
colorscheme gruvbox
set background=dark
filetype on
set signcolumn=yes "强制显示侧边栏，防止时有时无
syntax on
set shiftwidth=4
set tabstop=4
let g:ycm_clangd_binary_path='clangd'
set number



" 文件树的设置
nmap <Leader><Leader> :NERDTreeToggle<CR>
let NERDTreeWinSize=32
" 设置NERDTree子窗口宽度
let NERDTreeWinPos="right"      " 设置NERDTree子窗口位置
let NERDTreeShowHidden=1        " 显示隐藏文件
let NERDTreeMinimalUI=1         " NERDTree 子窗口中不显示冗余帮助信息
" 绑定F2到NERDTreeToggle
map <F2> :NERDTreeToggle<CR>


" YouCompleteMe
set runtimepath+=~/.vim/bundle/YouCompleteMe
let g:ycm_collect_identifiers_from_tags_files = 1           " 开启 YCM 基于标签引擎
let g:ycm_collect_identifiers_from_comments_and_strings = 1 " 注释与字符串中的内容也用于补全
let g:syntastic_ignore_files=[".*\.py$"]
let g:ycm_seed_identifiers_with_syntax = 1                  " 语法关键字补全
let g:ycm_complete_in_comments = 1
let g:ycm_confirm_extra_conf = 0
let g:ycm_key_list_select_completion = ['<c-n>', '<Down>']  " 映射按键, 没有这个会拦截掉tab, 导致其他插件的tab不能用.
let g:ycm_key_list_previous_completion = ['<c-p>', '<Up>']
let g:ycm_complete_in_comments = 1                          " 在注释输入中也能补全
let g:ycm_complete_in_strings = 1                           " 在字符串输入中也能补全
let g:ycm_collect_identifiers_from_comments_and_strings = 1 " 注释和字符串中的文字也会被收入补全
let g:ycm_global_ycm_extra_conf='~/.vim/bundle/YouCompleteMe/third_party/ycmd/cpp/ycm/.ycm_extra_conf.py'
let g:ycm_show_diagnostics_ui = 0                           " 禁用语法检查
inoremap <expr> <CR> pumvisible() ? "\<C-y>" : "\<CR>" |            " 回车即选中当前项
nnoremap <c-j> :YcmCompleter GoToDefinitionElseDeclaration<CR>|     " 跳转到定义处
"let g:ycm_min_num_of_chars_for_completion=2                 " 从第2个键入字符就开始罗列匹配项
let g:ycm_global_ycm_extra_conf = "~/.vim/plugged/YouCompleteMe/third_party/ycmd/cpp/ycm/.ycm_extra_conf.py"



"" airline
let laststatus = 2
let g:airline_powerline_fonts = 1
let g:airline_theme = "dark"
let g:airline#extensions#tabline#enabled = 1



"" vim-monokai
" colorscheme monokai

"" vim-markdown
" Github风格markdown语法
let g:vim_markdown_no_extensions_in_markdown = 1

"" vim-cpp-enhanced-highlight
let g:cpp_class_scope_highlight = 1
let g:cpp_member_variable_highlight = 1
let g:cpp_class_decl_highlight = 1
let g:cpp_experimental_template_highlight = 1

" 自定义error和warning图标
let g:ale_sign_error = '✗'
let g:ale_sign_warning = '⚡'
" 显示Linter名称,出错或警告等相关信息
let g:ale_echo_msg_error_str = 'E'
let g:ale_echo_msg_warning_str = 'W'
let g:ale_echo_msg_format = '[%linter%] %s [%severity%]'
"设置状态栏显示的内容,这里必须添加%{ALEGetStatusLine()到状态栏里
"设置ale显示内容
let g:ale_statusline_format = ['✗ %d', '⚡ %d', '✔ OK']
" 对verilog、c++、c、python单独设置linter
let g:ale_linters = { 'verilog': ['vlog'],
\   'c++': ['clangd'],
\   'c': ['gcc'],
\   'python': ['pylint'],
\}


let g:ale_lint_on_text_changed = 1

let g:ale_set_loclist = 0
let g:ale_set_quickfix = 1


