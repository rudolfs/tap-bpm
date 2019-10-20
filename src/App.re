open Revery;

let init = app => {
  let _ = Revery.Log.listen((_, msg) => print_endline("LOG: " ++ msg));

  let win =
    App.createWindow(
      ~createOptions=
        WindowCreateOptions.create(
          ~width=172,
          ~height=72,
          ~resizable=false,
          ~maximized=false,
          ~backgroundColor=Theme.Color.grey1,
          (),
        ),
      app,
      "BPM",
    );

  let _ = UI.start(win, <Bpm />);

  ();
};

App.start(init);
