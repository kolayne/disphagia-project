from typing import Dict, Any, Iterator, Iterable, Tuple
from functools import reduce


def flatten_dict(data: Dict[str, Any], *, sep: str = '.', _prefix: str = '') -> Iterator[Tuple[str, Any]]:
    """Flatten dict by promoting all nested dictionary values to the first level.
    
    Returns a sequence of key-value pairs (can be converted to a `dict`) that is a flattened given
    dictionary: values of the returned sequence are all non-`dict` values that can be found in
    the given dictionary (promoted values); keys of the returned sequence are constructed as
    a contatenation of all keys needed to access a particular promoted value, joined withi
    `sep` (by default, a dot).

    The sequence is sorted lexicographically in keys (which are assumed to be `str`s).

    It is assumed that keys of the given dictionary or any of its subdictionaries do
    not contain `sep`.

    WARNING: no deep copying is performed. All objects other than instances of
    (derived classes of) `dict` remain as-is.

    Args:
        data: Dictionary to flatten.
        sep: String to join keys of nested dictionaries with.
        _prefix: String to prefix all keys with.

    Returns:
        Generator object that produces a sequence of sorted key-value pairs corresponding
        to elements of `data` or its nested elements.

    Example:
        ```
        >>> dict(flatten_dict({'a': 123, 'b': {'e': 456, 'c': {}, 'd': {'x': None}}}))
        {'a': 123, 'b.d.x': None, 'b.e': 456}
        ```
    """
    source_keys = sorted(data.keys())
    for key in source_keys:
        val = data[key]
        if isinstance(val, dict):
            yield from flatten_dict(val, sep=sep, _prefix=_prefix+key+sep)
        else:
            yield _prefix+key, val


def extend_dicts_keys(dicts_: Iterable[Dict[Any, Any]], placeholder_value: Any = None) -> Iterator[Dict[Any, Any]]:
    """Extends all given dictionaries to have the same (super)set of keys.

    Inserts some new elements to the given dictionaries and returns them such that
    after the operation all dictionaries have the same set of keys that is the superset
    of keys of each of them. `placeholder_value` is assigned as a value where a key is missing.

    Args:
        dicts_: Iterable with dictionaries to process.
        placeholder_value: Value to set in the dictionaries to correspond to the missing keys.

    Returns:
        Generator object that produces the sequence of extended dictionaries.
    """
    dicts = list(dicts_)
    super_keys = reduce(lambda a, b: a.union(b),
                        map(lambda d: d.keys(), dicts),
                        set())
    placeholders = {key: placeholder_value for key in super_keys}
    return ({**placeholders, **true_data} for true_data in dicts)
