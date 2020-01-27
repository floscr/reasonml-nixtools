open Cmdliner;

let revolt_t = Term.(const(revolt) $ const());
let revolt = () => print_endline("Revolt!");

let () = Term.exit @@ Term.eval((revolt_t, Term.info("revolt")), HELLO);
