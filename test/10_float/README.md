# nan-inf

Exercises `+ - * == > <` over `-1, 0, 1, -inf, +inf, nan, -nan` and prints the
results with `mulle_printf( "%g")`.

## Why there is a `.cat` filter and a `.darwin` golden

The arithmetic is identical everywhere (IEEE 754). What differs per platform is
the **sign bit** of NaN and zero, which `%g` faithfully renders:

- `-NAN`, and invalid ops like `0 * inf`, `inf - inf`, `-inf - -inf`, print as
  `-nan` on darwin/x86_64 but `nan` elsewhere.
- `-1 * 0` prints as `-0` on darwin but `0` elsewhere.

`nan-inf.cat` is the output filter the harness runs *before* diffing. It
normalizes away the noise:

- `-nan` -> `nan`
- most `-0` / `-0.000...` -> `0`

The harness pipeline is: **run exe -> pipe through `nan-inf.cat` -> diff against golden**.

So the golden files must contain the **filtered** output, not the raw program
output.

## The golden files

- `nan-inf.stdout`        : platform-neutral golden (fully filtered).
- `nan-inf.stdout.darwin` : darwin golden. A platform suffix overrides the base.

The darwin golden exists only because the `.cat` regex does not strip a trailing
`-0` at end-of-line, so two lines survive filtering on darwin:

```
-1 * 0 = -0
0 * -1 = -0
```

Everything else is identical to the base golden.

## Gotchas when regenerating (read before you "golden")

- `mulle-sde test run --golden-stdout ...` captures the **raw** program stdout,
  i.e. *before* the `.cat` filter. Dropping that straight into a golden file
  reintroduces `-nan`/`-0` and the test will fail again. This is exactly how a
  stale `nan-inf.stdout.darwin` full of `-nan` crept in and broke the darwin run.
- The correct golden is the output **after** the filter:

  ```sh
  # from this directory, with the exe already crafted
  ./nan-inf.exe | ./nan-inf.cat > nan-inf.stdout.darwin   # darwin
  ```

  then confirm it differs from `nan-inf.stdout` only on the two `-0` lines above.
- Do not hand-edit goldens to paper over a real regression (see the testing
  howto). This case is not a regression: the filtered comparison is the intended
  contract, so the golden must equal the filtered stream.

## If you want to drop the `.darwin` file entirely

Fix `nan-inf.cat` so its zero-normalization also collapses a trailing `-0` at
end-of-line. Then the base `nan-inf.stdout` covers darwin and
`nan-inf.stdout.darwin` can be removed.
