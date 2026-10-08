// mtp-drop: send files to a Kindle's documents folder in one MTP session.
// usage: mtp-drop [-r] file...
// -r first deletes any file in documents with the exact same name.
// kindle-drop builds this on first run.
#include <libgen.h>
#include <libmtp.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
  int replace = argc > 1 && strcmp(argv[1], "-r") == 0;
  int first = replace ? 2 : 1;
  if (first >= argc) { fprintf(stderr, "usage: mtp-drop [-r] file...\n"); return 2; }

  // Folder-by-folder listing needs an uncached device.
  LIBMTP_Init();
  LIBMTP_raw_device_t *raw; int nraw = 0;
  if (LIBMTP_Detect_Raw_Devices(&raw, &nraw) != 0 || nraw < 1) { printf("no device\n"); return 1; }
  LIBMTP_mtpdevice_t *dev = LIBMTP_Open_Raw_Device_Uncached(&raw[0]);
  if (!dev) { printf("could not open device\n"); return 1; }

  uint32_t docs = 0, storage = 0;
  for (LIBMTP_file_t *f = LIBMTP_Get_Files_And_Folders(dev, 0, LIBMTP_FILES_AND_FOLDERS_ROOT); f; f = f->next)
    if (f->filetype == LIBMTP_FILETYPE_FOLDER && strcmp(f->filename, "documents") == 0) {
      docs = f->item_id; storage = f->storage_id;
    }
  if (!docs) { printf("no documents folder\n"); LIBMTP_Release_Device(dev); return 1; }
  LIBMTP_file_t *kids = replace ? LIBMTP_Get_Files_And_Folders(dev, storage, docs) : NULL;

  int ret = 0;
  for (int i = first; i < argc; i++) {
    struct stat sb;
    if (stat(argv[i], &sb) != 0) { printf("missing %s\n", argv[i]); ret = 1; continue; }
    char buf[PATH_MAX];
    strncpy(buf, argv[i], sizeof buf - 1); buf[sizeof buf - 1] = 0;
    char *name = basename(buf);

    // Exact names only. 0xFFFFFFFF would mean "every object" to the device.
    int skip = 0;
    for (LIBMTP_file_t *f = kids; f; f = f->next)
      if (f->filetype != LIBMTP_FILETYPE_FOLDER && strcmp(f->filename, name) == 0
          && f->item_id != 0 && f->item_id != 0xFFFFFFFFu) {
        if (LIBMTP_Delete_Object(dev, f->item_id) == 0) printf("replaced %s\n", name);
        else { printf("delete failed %s\n", name); ret = 1; skip = 1; }
      }
    if (skip) continue;

    LIBMTP_file_t *g = LIBMTP_new_file_t();
    g->filename = strdup(name);
    g->filesize = sb.st_size;
    g->filetype = LIBMTP_FILETYPE_UNKNOWN;
    g->parent_id = docs;
    g->storage_id = storage;
    int r = LIBMTP_Send_File_From_File(dev, argv[i], g, NULL, NULL);
    printf(r == 0 ? "sent %s\n" : "send failed %s\n", name);
    if (r) ret = 1;
    LIBMTP_destroy_file_t(g);
  }

  LIBMTP_Release_Device(dev);
  return ret;
}
