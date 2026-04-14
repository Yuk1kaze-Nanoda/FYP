const {
  requestBackend,
  saveAuth,
  getStoredToken,
  clearAuth
} = require('../../utils/backend');

Page({
  data: {
    phone: '',
    password: '',
    mode: 'login',
    loading: false
  },

  onLoad() {
    const token = getStoredToken();
    if (token) {
      this.verifyAndJump();
    }
  },

  onPhoneChange(event) {
    this.setData({
      phone: String(event.detail || '').replace(/\D/g, '').slice(0, 11)
    });
  },

  onPasswordChange(event) {
    this.setData({
      password: String(event.detail || '')
    });
  },

  toggleMode() {
    this.setData({
      mode: this.data.mode === 'login' ? 'register' : 'login'
    });
  },

  onSubmit() {
    const phone = this.data.phone.trim();
    const password = this.data.password;

    if (!/^1\d{10}$/.test(phone)) {
      wx.showToast({
        title: 'Invalid phone number',
        icon: 'none'
      });
      return;
    }

    if (!password || password.length < 6) {
      wx.showToast({
        title: 'Password too short',
        icon: 'none'
      });
      return;
    }

    this.setData({ loading: true });

    if (this.data.mode === 'register') {
      this.register(phone, password);
      return;
    }

    this.login(phone, password);
  },

  register(phone, password) {
    requestBackend({
      url: '/api/auth/register',
      method: 'POST',
      data: { phone, password }
    })
      .then(() => {
        wx.showToast({
          title: 'Register success',
          icon: 'none'
        });
        this.setData({
          mode: 'login'
        });
      })
      .catch((error) => {
        wx.showToast({
          title: error.message || 'Register failed',
          icon: 'none'
        });
      })
      .finally(() => {
        this.setData({ loading: false });
      });
  },

  login(phone, password) {
    requestBackend({
      url: '/api/auth/login',
      method: 'POST',
      data: { phone, password }
    })
      .then((response) => {
        saveAuth(response.token, response.user);
        wx.showToast({
          title: 'Login success',
          icon: 'success'
        });

        setTimeout(() => {
          wx.reLaunch({
            url: '/pages/index/index'
          });
        }, 250);
      })
      .catch((error) => {
        wx.showToast({
          title: error.message || 'Login failed',
          icon: 'none'
        });
      })
      .finally(() => {
        this.setData({ loading: false });
      });
  },

  verifyAndJump() {
    requestBackend({
      url: '/api/auth/me',
      method: 'GET',
      auth: true
    })
      .then(() => {
        wx.reLaunch({
          url: '/pages/index/index'
        });
      })
      .catch(() => {
        clearAuth();
      });
  }
});
