const BACKEND_BASE_URL = 'http://127.0.0.1:8080';

const TOKEN_KEY = 'token';
const USER_KEY = 'user';

function getStoredToken() {
  return wx.getStorageSync(TOKEN_KEY) || '';
}

function getStoredUser() {
  return wx.getStorageSync(USER_KEY) || null;
}

function saveAuth(token, user) {
  wx.setStorageSync(TOKEN_KEY, token || '');
  wx.setStorageSync(USER_KEY, user || null);
}

function clearAuth() {
  wx.removeStorageSync(TOKEN_KEY);
  wx.removeStorageSync(USER_KEY);
}

function extractMessage(data) {
  if (!data) {
    return '';
  }
  if (typeof data === 'string') {
    return data;
  }
  if (typeof data.message === 'string') {
    return data.message;
  }
  if (typeof data.error === 'string') {
    return data.error;
  }
  return '';
}

function mapStatusMessage(statusCode, url, backendMessage) {
  const normalized = String(backendMessage || '').trim();
  if (normalized && !normalized.startsWith('Request failed')) {
    return normalized;
  }

  if (url === '/api/auth/login') {
    if (statusCode === 404) {
      return 'Account does not exist';
    }
    if (statusCode === 401) {
      return 'Password is incorrect';
    }
  }

  if (url === '/api/auth/register' && statusCode === 409) {
    return 'Phone already registered';
  }

  switch (statusCode) {
    case 400:
      return 'Invalid request data';
    case 401:
      return 'Unauthorized';
    case 403:
      return 'Forbidden';
    case 404:
      return 'Resource not found';
    case 409:
      return 'Conflict request';
    case 500:
      return 'Server internal error';
    default:
      return `Request failed (${statusCode})`;
  }
}

function mapNetworkMessage(errMsg) {
  const raw = String(errMsg || '');
  const lower = raw.toLowerCase();

  if (lower.includes('url not in domain list') || lower.includes('not in domain list')) {
    return 'Backend URL not in domain whitelist; disable URL check in WeChat DevTools for local testing';
  }

  if (lower.includes('timeout')) {
    return 'Request timeout, please retry';
  }

  if (lower.includes('refused') || lower.includes('fail')) {
    return 'Cannot connect to backend service';
  }

  return raw || 'Network error';
}

function requestBackend(options) {
  const {
    url,
    method = 'GET',
    data = {},
    auth = false
  } = options;

  return new Promise((resolve, reject) => {
    const header = {
      'content-type': 'application/json'
    };

    if (auth) {
      const token = getStoredToken();
      if (token) {
        header.Authorization = `Bearer ${token}`;
      }
    }

    wx.request({
      url: `${BACKEND_BASE_URL}${url}`,
      method,
      data,
      header,
      success(res) {
        if (res.statusCode >= 200 && res.statusCode < 300) {
          resolve(res.data);
          return;
        }

        const backendMessage = extractMessage(res.data);
        const message = mapStatusMessage(res.statusCode, url, backendMessage);

        if (res.statusCode === 401 && auth) {
          clearAuth();
        }

        reject({
          statusCode: res.statusCode,
          message,
          data: res.data
        });
      },
      fail(err) {
        reject({
          statusCode: 0,
          message: mapNetworkMessage(err && err.errMsg),
          data: err
        });
      }
    });
  });
}

module.exports = {
  BACKEND_BASE_URL,
  getStoredToken,
  getStoredUser,
  saveAuth,
  clearAuth,
  requestBackend
};