# kindle-drop

Put ebooks on a USB-connected Kindle from a Mac. One command.

```bash
kindle-drop book.epub another.epub
```

Newer Kindles connect over MTP, so they never show up in Finder. Kindles also can't open sideloaded EPUB files. kindle-drop fixes both: it converts each book to AZW3 with Calibre, then copies it to the Kindle's `documents/` folder with libmtp.

## Install

```bash
brew install libmtp poppler imagemagick
brew install --cask calibre
git clone https://github.com/sgwstudio/kindle-drop.git
ln -s "$PWD/kindle-drop/kindle-drop" /opt/homebrew/bin/kindle-drop
```

## Use

1. Plug in the Kindle and unlock it.
2. Close anything else that might grab it (Calibre, Android File Transfer, OpenMTP).
3. Run `kindle-drop` with one or more files.

When the send finishes, the Kindle ejects itself. It leaves USB mode and shows a battery notice while it indexes the new books. That's expected. To send more, unplug and replug it.

AZW3, MOBI, PDF and TXT files go across as-is. Calibre converts everything else (EPUB, FB2, DOCX...) to AZW3 first.

Scanned PDFs get cleaned on the way. Each page becomes one black-and-white image at screen height, with white paper and black text. Scans from sites like Internet Archive stack several image layers per page, which makes a Kindle freeze and the page look dark. Cleaned, they open fast and stay small.

## License

MIT
