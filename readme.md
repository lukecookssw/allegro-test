# Running this silly little thing.

This is a pet project, so a polished "F5 experience" isn't a priority. If you'd like to run this, then you'll need the following:

- [gcc](https://gcc.gnu.org/)
- [Allegro 5](https://www.allegro.cc/) with the following modules:
    - allegro_primitives-5
    - allegro_font-5
    - allegro_ttf-5

Installation of Allegro and these modules depends on your distro. Some have meta packages, some don't. idk man...

## Building
- `chmod +x build.sh`
- `./build.sh`

## Running
- `./main.out`. Bet you couldn't figure *that* out.

## Debugging
- Currently set up for GDB
- It should "just work" in Zed. Unless you're using WSL on Windows. See [this issue](https://github.com/zed-industries/zed/issues/41753)
- If you're relying on the VS Code setup, you'll need the CPP tools extension (this works with WSL in VS Code)

## Issues?
Of course there are issues. It's a pet project to hack away in C and Allegro. It's a learning curve. A *steep* learning curve. And I'm out of shape.