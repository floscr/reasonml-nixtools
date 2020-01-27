/* Implementation of the command, we just print the args. */

type prompt =
  | Always
  | Once
  | Never;
let prompt_str =
  fun
  | Always => "always"
  | Once => "once"
  | Never => "never";

let rm = (prompt, recurse, files) =>
  Printf.printf(
    "prompt = %s\nrecurse = %B\nfiles = %s\n",
    prompt_str(prompt),
    recurse,
    String.concat(", ", files),
  );

/* Command line interface */

open Cmdliner;

let files = Arg.(non_empty & pos_all(file, []) & info([], ~docv="FILE"));
let prompt = {
  let doc = "Prompt before every removal.";
  let always = (Always, Arg.info(["i"], ~doc));
  let doc = "Ignore nonexistent files and never prompt.";
  let never = (Never, Arg.info(["f", "force"], ~doc));
  let doc = "Prompt once before removing more than three files, or when\n             removing recursively. Less intrusive than $(b,-i), while\n             still giving protection against most mistakes.";

  let once = (Once, Arg.info(["I"], ~doc));
  Arg.(last & vflag_all([Always], [always, never, once]));
};

let recursive = {
  let doc = "Remove directories and their contents recursively.";
  Arg.(value & flag & info(["r", "R", "recursive"], ~doc));
};

let cmd = {
  let doc = "remove files or directories";
  let man = [
    `S(Manpage.s_description),
    `P(
      "$(tname) removes each specified $(i,FILE). By default it does not\n        remove directories, to also remove them and their contents, use the\n        option $(b,--recursive) ($(b,-r) or $(b,-R)).",
    ),
    `P(
      "To remove a file whose name starts with a `-', for example\n        `-foo', use one of these commands:",
    ),
    `P("rm -- -foo"),
    `Noblank,
    `P("rm ./-foo"),
    `P(
      "$(tname) removes symbolic links, not the files referenced by the\n        links.",
    ),
    `S(Manpage.s_bugs),
    `P("Report bugs to <hehey at example.org>."),
    `S(Manpage.s_see_also),
    `P("$(b,rmdir)(1), $(b,unlink)(2)"),
  ];

  (
    Term.(const(rm) $ prompt $ recursive $ files),
    Term.info("rm", ~version="v1.0.4", ~doc, ~exits=Term.default_exits, ~man),
  );
};

let () = Term.(exit @@ eval(cmd));
