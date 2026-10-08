# kindle-drop

Put ebooks on a USB-connected Kindle from a Mac. One command.

```bash
kindle-drop book.epub another.epub
```

Newer Kindles connect over MTP, so they never show up in Finder. Kindles also can't open sideloaded EPUB files. kindle-drop fixes both: it converts each book to AZW3 with Calibre, then copies it to the Kindle's `documents/` folder with libmtp.

## Install

```bash
brew install libmtp
brew install --cask calibre
git clone https://github.com/sgwstudio/kindle-drop.git
ln -s "$PWD/kindle-drop/kindle-drop" /opt/homebrew/bin/kindle-drop
```

## Use

1. Plug in the Kindle and unlock it.
2. Close anything else that might grab it (Calibre, Android File Transfer, OpenMTP).
3. Run `kindle-drop` with one or more files.

AZW3, MOBI, PDF and TXT files go across as-is. Calibre converts everything else (EPUB, FB2, DOCX...) to AZW3 first.

## License

MIT
