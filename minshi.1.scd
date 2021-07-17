minshi(1)

# NAME

minshi - listen for output connect/disconnect events

# DESCRIPTION

*minshi*

# SYNOPSIS

minshi is a Wayland daemon that prints output connect/disconnect events to
standard output.  Each line will contain either:
```
connect <TAB> <output name>
```
or
```
disconnect <TAB> <output name>
```

# AUTHORS

Maintained by Devin J. Pohly <djpohly@gmail.com> and based almost entirely
on the kanshi utility by Simon Ser <contact@emersion.fr>.  For more information
about minshi development, see https://github.com/djpohly/minshi.
