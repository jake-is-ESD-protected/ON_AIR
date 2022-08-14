import os

css_sig = "</style>"
js_sig = "</script>"

with open(os.getcwd() + "/website/base.html") as f_html:
    c_html = f_html.read()
    idx = c_html.find(css_sig)

    with open(os.getcwd() + "/website/style.css") as f_css:
        c_css = f_css.read()
        c_html = c_html[:idx] + c_css + '\n' + c_html[idx:]

    idx = c_html.find(js_sig)

    with open(os.getcwd() + "/website/ajax.js") as f_js:
        c_js = f_js.read()
        c_html = c_html[:idx] + c_js + '\n' + c_html[idx:]

# write the html-file for test-viewing in the browser
with open (os.getcwd() + "/website/index.html", "w") as f_index:
    f_index.write(c_html)

# write h-file for compilation
with open (os.getcwd() + "/lib/general/index.h", "w") as f_index:

    c_h = "#ifndef _INDEX_H_\n#define _INDEX_H_\n\n"
    c_h += 'const char ws_index[] = R"=====(\n'
    c_h += c_html
    c_h += '\n)=====";\n\n#endif'

    f_index.write(c_h) 