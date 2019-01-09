"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.camelCase = camelCase;
exports.validateAccountName = validateAccountName;
var snakeCaseRe = /_([a-z])/g;
function camelCase(str) {
  return str.replace(snakeCaseRe, function (_m, l) {
    return l.toUpperCase();
  });
}

function validateAccountName(value) {
  var i = void 0,
      label = void 0,
      len = void 0,
      suffix = void 0;

  suffix = "Account name should ";
  if (!value) {
    return suffix + "not be empty.";
  }
  var length = value.length;
  if (length < 3) {
    return suffix + "be longer.";
  }
  if (length > 16) {
    return suffix + "be shorter.";
  }
  if (/\./.test(value)) {
    suffix = "Each account segment should ";
  }
  var ref = value.split(".");
  for (i = 0, len = ref.length; i < len; i++) {
    label = ref[i];
    if (!/^[a-z]/.test(label)) {
      return suffix + "start with a letter.";
    }
    if (!/^[a-z0-9-]*$/.test(label)) {
      return suffix + "have only letters, digits, or dashes.";
    }
    if (/--/.test(label)) {
      return suffix + "have only one dash in a row.";
    }
    if (!/[a-z0-9]$/.test(label)) {
      return suffix + "end with a letter or digit.";
    }
    if (!(label.length >= 3)) {
      return suffix + "be longer";
    }
  }
  return null;
}