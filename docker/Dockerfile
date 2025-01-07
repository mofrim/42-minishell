FROM nixos/nix:latest
COPY ./default.nix .
COPY ./shell.nix .
RUN nix-env -i -f ./default.nix
CMD ["nix-shell"]

